"""One place for the port's paths and pins.

Values come from, in order of precedence: environment variables, the ignored
`local.json` beside `upstreams.json` (or the file named by VECTORCRAFT_LOCAL),
and the defaults below. Workstation paths have no defaults: a missing value is
reported with the variable that sets it, never guessed.
"""
import json
import os
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
ABI = "v11"

# key: (environment variable, default, description)
SETTINGS = {
    "native_profile": ("VECTORCRAFT_NATIVE_PROFILE", "v11", "native profile: v11 or official v1"),
    "official_sdk": ("VECTORCRAFT_OFFICIAL_SDK", None, "qualified official mainline Rust installation prefix"),
    "official_libc_archive": ("VECTORCRAFT_OFFICIAL_LIBC_ARCHIVE", None, "original libc crate archive for the official profile"),
    "aros_gcc": ("VECTORCRAFT_AROS_GCC", None, "x86_64 AROS C compiler driver matching the SDK"),
    "aros_ar": ("VECTORCRAFT_AROS_AR", None, "archiver from the same toolchain"),
    "aros_strip": ("VECTORCRAFT_AROS_STRIP", None, "strip from the same toolchain (optional)"),
    "aros_sdk": ("VECTORCRAFT_AROS_SDK", None, "native ABI-matched C SDK used as --sysroot"),
    "rust_aros": ("VECTORCRAFT_RUST_AROS", None, "rust-aros Git checkout (target template, libc-aros)"),
    "rust_aros_commit": ("VECTORCRAFT_RUST_AROS_COMMIT", "7b00bb2aab0622788e60601f39f5cf79b63c3509",
                         "rust-aros commit whose target template and libc-aros are used"),
    "aros_src": ("VECTORCRAFT_AROS_SRC", None, "deadwood2/AROS (ABIv11) Git checkout; read-only source of the pthread link library"),
    "aros_src_commit": ("VECTORCRAFT_AROS_SRC_COMMIT", "5376f09bc1ed0c36a12fbb096406013f8a046e89",
                        "commit whose compiler/pthread matches the SDK's libpthread.a"),
    "egui_aros": ("VECTORCRAFT_EGUI_AROS", None, "egui-aros Git checkout; its commit pinned in upstreams.json is built"),
    "egui_upstream_root": ("VECTORCRAFT_EGUI_UPSTREAM_ROOT", None, "read-only prepared egui-aros upstream repositories"),
    "toolchain": ("VECTORCRAFT_TOOLCHAIN", None, "rustup name of the private AROS std toolchain"),
    "rustc_version": ("VECTORCRAFT_RUSTC_VERSION", "rustc 1.100.0-nightly (0dfb098f3 2026-08-31)",
                      "exact `rustc --version` the toolchain must report"),
    "cargo_home": ("VECTORCRAFT_CARGO_HOME", "deps/aros-x86_64-abiv11-pc/cargo-home", "private CARGO_HOME (relative: to the repository)"),
    "build_root": ("VECTORCRAFT_BUILD_ROOT", "build/aros-x86_64-abiv11-pc", "build outputs (relative: to the repository)"),
    "panic": ("VECTORCRAFT_PANIC", "unwind", "native panic profile: unwind or abort"),
}


class ConfigError(Exception):
    pass


def _local():
    path = Path(os.environ.get("VECTORCRAFT_LOCAL", REPO / "local.json"))
    if not path.exists():
        return {}, None
    try:
        data = json.loads(path.read_text())
    except (OSError, ValueError) as error:
        raise ConfigError(f"{path}: {error}") from error
    unknown = set(data) - set(SETTINGS)
    if unknown:
        raise ConfigError(f"{path}: unknown keys {sorted(unknown)}")
    return data, path


def load(required=()):
    """Returns the resolved settings; raises ConfigError naming what is missing."""
    local, local_path = _local()
    out = {}
    for key, (env, default, _) in SETTINGS.items():
        value = os.environ.get(env) or local.get(key) or default
        out[key] = value
    paths = ("official_sdk", "official_libc_archive", "aros_gcc", "aros_ar", "aros_strip", "aros_sdk", "rust_aros", "aros_src", "egui_aros", "egui_upstream_root", "cargo_home", "build_root")
    for key in paths:
        if out[key] and not os.path.isabs(out[key]):
            base = REPO if os.environ.get(SETTINGS[key][0]) or key not in local else (local_path.parent if local_path else REPO)
            out[key] = str((base / out[key]).resolve())
    missing = [key for key in required if not out.get(key)]
    if missing:
        lines = [f"  {k}: set {SETTINGS[k][0]} or \"{k}\" in local.json ({SETTINGS[k][2]})" for k in missing]
        raise ConfigError("missing configuration:\n" + "\n".join(lines))
    if out["native_profile"] not in ("v11", "v1"):
        raise ConfigError("native_profile must be v11 or v1")
    if out["panic"] not in ("unwind", "abort"):
        raise ConfigError("panic must be unwind or abort")
    return out


def describe():
    local, path = _local()
    rows = []
    for key, (env, default, text) in SETTINGS.items():
        source = "env" if os.environ.get(env) else ("local" if key in local else ("default" if default else "unset"))
        rows.append(f"{key:18} {source:8} {env}  ({text})")
    return f"local file: {path or 'none'}\n" + "\n".join(rows)
