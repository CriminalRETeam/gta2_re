## Contributing
Anyone who wishes to contribute is encouraged to join the project's [Discord](https://discord.gg/4mTfhQKNQM), where most of the communication happens.

## Building

### Prerequisites 
- Python >= 3.7
- Wine (For building on Linux)

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

```bash
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
python3 vc6_setup.py
python3 build.py
```
