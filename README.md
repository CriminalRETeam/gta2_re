## Contributing
Anyone who wishes to contribute is encouraged to join the project's [Discord](https://discord.gg/4mTfhQKNQM), where most of the communication happens.

## Building

Clone the repository with the `--recursive` flag:

```
git clone --recursive https://github.com/CriminalRETeam/gta2_re.git
```

### Windows 

```
python vc6_setup.py
python build.py
```

### Linux

*In order to build on linux you'll also need to install wine

```
python3 -m venv ~/venv
~/venv/bin/pip install -r requirements.txt
~/venv/bin/python vc6_setup.py
~/venv/bin/python build.py
```
