import argparse
from pathlib import Path
from get_platform import get_platform
import zipfile
import io
import requests
import stat

root_path = Path(__file__).parent.parent


platform = get_platform()
if platform is None:
    exit(1)

parser = argparse.ArgumentParser()
parser.add_argument("tool")
parser.add_argument("tag")
parser.add_argument("--path", "-p", type=Path, required=True)
args = parser.parse_args()


def dsdrom_url(tag: str) -> str:
    return f'https://github.com/AetiasHax/ds-rom/releases/download/{tag}/dsrom-{platform.system}-{platform.machine}{platform.exe}'

TOOLS = {
    "dsrom": dsdrom_url,
}

download_url = TOOLS[args.tool](args.tag)
print(f'\nDownloading {args.tool} {args.tag}...')
response = requests.get(download_url)
if download_url.endswith('.zip'):
    zip_file = zipfile.ZipFile(io.BytesIO(response.content))
    zip_file.extractall(args.path / args.tool)
else:
    out_path: Path = args.path / args.tool
    with out_path.open('wb') as f:
        f.write(response.content)
    out_path.chmod(out_path.stat().st_mode | stat.S_IEXEC)
