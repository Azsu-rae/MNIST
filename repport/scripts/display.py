#!/usr/bin/env python3
"""Render MNIST digits from IDX files to PNGs.

Usage:
    python scripts/display.py [--split train|test] [--count N] [--out DIR]

Examples:
    python scripts/display.py                    # first 10 train digits → images/
    python scripts/display.py --count 25
    python scripts/display.py --split test --count 5 --out images/test
"""
import argparse
import struct
from pathlib import Path

import numpy as np
from PIL import Image

ROOT = Path(__file__).resolve().parent.parent
MNIST_DIR = ROOT.parent / "dataset" / "MNIST"

FILES = {
    "train": ("train-images.idx3-ubyte", "train-labels.idx1-ubyte"),
    "test":  ("t10k-images.idx3-ubyte",  "t10k-labels.idx1-ubyte"),
}


def load_idx_images(path: Path) -> np.ndarray:
    with path.open("rb") as f:
        magic, n, rows, cols = struct.unpack(">IIII", f.read(16))
        if magic != 0x00000803:
            raise ValueError(f"{path}: bad magic 0x{magic:08x}")
        data = np.frombuffer(f.read(), dtype=np.uint8)
    return data.reshape(n, rows, cols)


def load_idx_labels(path: Path) -> np.ndarray:
    with path.open("rb") as f:
        magic, n = struct.unpack(">II", f.read(8))
        if magic != 0x00000801:
            raise ValueError(f"{path}: bad magic 0x{magic:08x}")
        return np.frombuffer(f.read(), dtype=np.uint8)


def main() -> None:
    p = argparse.ArgumentParser()
    p.add_argument("--split", choices=["train", "test"], default="train")
    p.add_argument("--count", type=int, default=10)
    p.add_argument("--out", type=Path, default=ROOT / "images")
    p.add_argument("--scale", type=int, default=8,
                   help="upscale factor for easier viewing (default 8)")
    args = p.parse_args()

    img_name, lbl_name = FILES[args.split]
    images = load_idx_images(MNIST_DIR / img_name)
    labels = load_idx_labels(MNIST_DIR / lbl_name)

    args.out.mkdir(parents=True, exist_ok=True)

    n = min(args.count, len(images))
    for i in range(n):
        arr = images[i]
        if args.scale != 1:
            arr = np.kron(arr, np.ones((args.scale, args.scale), dtype=np.uint8))
        out = args.out / f"{args.split}_{i:05d}_label{labels[i]}.png"
        Image.fromarray(arr, mode="L").save(out)

    print(f"wrote {n} images to {args.out}")


if __name__ == "__main__":
    main()
