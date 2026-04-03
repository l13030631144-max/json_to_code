import shutil
import sys
from pathlib import Path

# Explicit source paths (adjust as needed)
SOURCE_PATHS = [
    r"D:\python_project\Qt_pj\json _to_code_by_skills\workspace\11标签-开关\build\Debug\TestDialog.dll",
    r"D:\python_project\Qt_pj\json _to_code_by_skills\workspace\11标签-开关\build\Debug\TestDialog.ui",
]
DESTINATION_DIR = r"D:\powercad\cad3.27\Release\PWBIN\Workbench\gui"
OVERWRITE = True


def copy_item(src: Path, dst_dir: Path, overwrite: bool = False) -> None:
    if not src.exists():
        raise FileNotFoundError(f"Source path does not exist: {src}")

    dst_dir.mkdir(parents=True, exist_ok=True)
    target = dst_dir / src.name

    if target.exists() and not overwrite:
        raise FileExistsError(f"Target already exists: {target}. Set OVERWRITE=True to replace it.")

    if src.is_file():
        shutil.copy2(src, target)
        print(f"Copied file: {src} -> {target}")
    elif src.is_dir():
        if target.exists() and overwrite:
            shutil.rmtree(target)
        shutil.copytree(src, target, dirs_exist_ok=overwrite)
        print(f"Copied directory: {src} -> {target}")
    else:
        raise ValueError(f"Unsupported path type: {src}")


def main() -> None:
    # Keep terminal output readable across different Windows code pages.
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    if hasattr(sys.stderr, "reconfigure"):
        sys.stderr.reconfigure(encoding="utf-8", errors="replace")

    destination = Path(DESTINATION_DIR).expanduser().resolve()
    if not SOURCE_PATHS:
        raise ValueError("SOURCE_PATHS cannot be empty. Provide at least one source path.")

    for raw_path in SOURCE_PATHS:
        source = Path(raw_path).expanduser().resolve()
        copy_item(source, destination, overwrite=OVERWRITE)


if __name__ == "__main__":
    main()
