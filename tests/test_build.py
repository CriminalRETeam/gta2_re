import io
import os
import tempfile
import unittest
from unittest import mock

import build


class FakeProcess:
    def __init__(self):
        self.stdout = io.StringIO("")

    def poll(self):
        return None

    def wait(self):
        return 0


class PosixWineInvocationTests(unittest.TestCase):
    def setUp(self):
        self.temp_dir = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp_dir.cleanup)
        self.build_dir = os.path.join(self.temp_dir.name, "build_vc6")
        os.makedirs(self.build_dir)
        self.vc6_env = ("Z:\\vc6\\Lib", "Z:\\vc6\\Include;", "/vc6/bin;/cmake/bin")

    def assert_safe_wine_invocation(self, popen):
        args, kwargs = popen.call_args
        command = args[0]

        self.assertIsInstance(command, list)
        self.assertEqual(command[:3], ["wine", "cmd", "/c"])
        self.assertEqual(len(command), 4)
        self.assertNotIn("shell", kwargs)
        self.assertEqual(kwargs["env"]["WINEDEBUG"], "-all")
        self.assertEqual(kwargs["env"]["WINEPATH"], self.vc6_env[2])
        self.assertEqual(kwargs["env"]["LIB"], self.vc6_env[0])
        self.assertEqual(kwargs["env"]["INCLUDE"], self.vc6_env[1])
        return command[3]

    @mock.patch.object(build.platform, "system", return_value="Linux")
    def test_vc6_environment_uses_literal_semicolon_separators(self, _system):
        with mock.patch.object(build, "CURRENT_DIRECTORY", "/tmp/gta2 re"):
            lib, include, path = build.get_vc6_env()

        self.assertNotIn(r"\;", include)
        self.assertNotIn(r"\;", path)
        self.assertEqual(include.count(";"), 3)
        self.assertEqual(path.count(";"), 2)
        self.assertTrue(lib.startswith("Z:"))

    @mock.patch.object(build, "check_duplicated_globals", return_value=True)
    @mock.patch.object(build, "build_cmake", return_value=0)
    @mock.patch.object(build.subprocess, "run")
    def test_reccmp_generation_failure_is_returned_to_caller(
        self, run, _build_cmake, _check_globals
    ):
        run.return_value = mock.Mock(returncode=7)

        with mock.patch.object(build.sys, "argv", ["build.py", "--reccmp"]):
            with self.assertRaises(SystemExit) as exit_context:
                build.main()

        self.assertEqual(exit_context.exception.code, 7)
        run.assert_called_once_with(
            [build.sys.executable, "reccmp/generate.py"],
            cwd=build.CURRENT_DIRECTORY,
        )

    @mock.patch.object(build.platform, "system", return_value="Linux")
    @mock.patch.object(build, "get_vc6_env")
    @mock.patch.object(build.subprocess, "Popen")
    def test_cmake_build_passes_complete_windows_command_as_one_argument(
        self, popen, get_vc6_env, _system
    ):
        popen.return_value = FakeProcess()
        get_vc6_env.return_value = self.vc6_env

        with mock.patch.object(build, "BUILD_DIRECTORY", self.build_dir), mock.patch.object(
            build, "BUILD_FOLDER_NAME", self.build_dir
        ):
            result = build.build_cmake(reccmp=False, core_count_to_use=2)

        self.assertEqual(result, 0)
        windows_command = self.assert_safe_wine_invocation(popen)
        self.assertIn('-G"NMake Makefiles JOM"', windows_command)
        self.assertIn("cmake --build . --target all -- -j 2", windows_command)

    @mock.patch.object(build.platform, "system", return_value="Linux")
    @mock.patch.object(build, "get_vc6_env")
    @mock.patch.object(build.subprocess, "Popen")
    def test_single_cpp_build_uses_environment_instead_of_shell_exports(
        self, popen, get_vc6_env, _system
    ):
        popen.return_value = FakeProcess()
        get_vc6_env.return_value = self.vc6_env

        with mock.patch.object(build, "BUILD_DIRECTORY", self.build_dir), mock.patch.object(
            build, "BUILD_FOLDER_NAME", self.build_dir
        ), mock.patch.object(build, "CURRENT_DIRECTORY", self.temp_dir.name):
            result = build.build_single_cpp("Player.cpp")

        self.assertEqual(result, 0)
        windows_command = self.assert_safe_wine_invocation(popen)
        self.assertIn("cl.exe @", windows_command)
        self.assertNotIn("export ", windows_command)


if __name__ == "__main__":
    unittest.main()
