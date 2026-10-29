# VectorCraft for AROS

Port of [VectorCraft](https://github.com/storytold/vectorcraft) (main of 2026-10-27, after 0.17.0), a vector drawing program written in Rust, to AROS.

Status: test release 0.17.0-510-g9997ff02-test1 (VectorCraft main at 9997ff02, 510 commits after 0.17.0) for the three targets below. Packages are on the [releases page](https://github.com/tomaszstaniak/aros-vectorcraft/releases).

Targets:

| Target | System |
|---|---|
| `aros-x86_64-abiv1-pc` | mainline AROS, x86_64 PC |
| `aros-x86_64-abiv11-pc` | ABIv11 distributions (AROS One), x86_64 PC |
| `aros-aarch64-abiv1-raspi` | native AROS on Raspberry Pi |

The upstream source is pinned in `upstreams.json`; AROS changes are kept as patches in `patches/vectorcraft/series`. See [docs](docs/README.md).

## Licence

VectorCraft is MIT OR Apache-2.0. The port's scripts and patches use the same terms.
