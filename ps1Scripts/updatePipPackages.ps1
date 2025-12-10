python -m pip install --upgrade pip
pip list --outdated --format=json |
    ConvertFrom-Json |
    ForEach-Object { pip install --upgrade $_.name }

