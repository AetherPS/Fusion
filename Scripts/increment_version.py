#!/usr/bin/env python3
"""
Fusion Version Incrementer
Automatically increments the build revision (third number) in fusion_version
"""

import re
import sys
from pathlib import Path

def increment_version(file_path):
    """
    Increment the build revision in FusionSysctl.cpp

    Args:
        file_path: Path to FusionSysctl.cpp

    Returns:
        tuple: (old_version, new_version) or (None, None) on error
    """
    try:
        # Read the file
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()

        # Pattern to match: char FusionSysctl::fusion_version[64] = "X.Y.Z";
        pattern = r'(char FusionSysctl::fusion_version\[64\] = ")(\d+)\.(\d+)\.(\d+)("\;)'

        def increment_build(match):
            """Increment the build number (third integer)"""
            prefix = match.group(1)
            major = match.group(2)
            minor = match.group(3)
            build = int(match.group(4))
            suffix = match.group(5)

            new_build = build + 1
            old_version = f"{major}.{minor}.{build}"
            new_version = f"{major}.{minor}.{new_build}"

            # Store versions for return
            increment_build.old_version = old_version
            increment_build.new_version = new_version

            return f"{prefix}{major}.{minor}.{new_build}{suffix}"

        # Perform the replacement
        new_content, count = re.subn(pattern, increment_build, content)

        if count == 0:
            print("Error: Could not find fusion_version line in file", file=sys.stderr)
            return None, None

        if count > 1:
            print("Warning: Found multiple fusion_version lines, updated all of them", file=sys.stderr)

        # Write back to file
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(new_content)

        return increment_build.old_version, increment_build.new_version

    except FileNotFoundError:
        print(f"Error: File not found: {file_path}", file=sys.stderr)
        return None, None
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        return None, None

def main():
    # Default path to FusionSysctl.cpp
    script_dir = Path(__file__).parent
    default_path = script_dir / ".." / "Fusion" / "Kernel" / "source" / "Misc" / "FusionSysctl.cpp"

    # Allow custom path from command line
    file_path = Path(sys.argv[1]) if len(sys.argv) > 1 else default_path

    if not file_path.exists():
        print(f"Error: File does not exist: {file_path}", file=sys.stderr)
        sys.exit(1)

    print(f"Incrementing version in: {file_path}")

    old_version, new_version = increment_version(file_path)

    if old_version and new_version:
        print(f"Version updated: {old_version} -> {new_version}")
        sys.exit(0)
    else:
        print("Failed to increment version", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()
