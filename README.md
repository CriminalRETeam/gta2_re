## Contributing
Anyone who wishes to contribute is encouraged to join the project's [Discord](https://discord.gg/4mTfhQKNQM), where most of the communication happens.

Also, you can find more info about the project (and how to effectively contribute) on [GTA2 RE Hub](https://valps.github.io/gta2-re-hub/).

## Building

### Prerequisites 
- Python >= 3.7
- `GTA2_ROOT` Environment variable pointing to your GTA2 installation
- Wine (For Linux/Mac)

Clone the repository with the `--recursive` flag:

```
git clone --recursive https://github.com/CriminalRETeam/gta2_re.git
```

### Windows 

```
pip install -r requirements.txt
python vc6_setup.py
python build.py
```

### Linux

```bash
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
python3 vc6_setup.py
python3 build.py
```

Optionally, you can automatically run the built exe by passing one of the following arguments to `build.py`:

- `--run_standalone`
- `--run_patched`

> [!IMPORTANT]
> To use `--run_patched`, you need the original GTA2 exe (called `10.5.exe`) to be within the `build_vc6` folder. However, the running script will try to setup it automatically (using the executable downloaded automatically after building for example) so you won't need to do it by yourself: just use `--run_patched` and it should work. Otherwise you can generate the patched exe yourself by running `ExePatcher.exe` within the `build_vc6` folder.
