#!/usr/bin/env python3
"""Summary of build/report.json (tools/bin/ninja report): matched code and functions, overall
and per category.

    tools/progress.py
"""
import json
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def row(name, m):
    code = int(m.get("total_code", 0))
    print(f"{name:<14} {m.get('matched_code_percent', 0):6.2f} % code "
          f"({int(m.get('matched_code', 0)):>8} / {code:>8} bytes), "
          f"{m.get('matched_functions', 0):>5} / {m.get('total_functions', 0)} functions, "
          f"fuzzy {m.get('fuzzy_match_percent', 0):6.2f} %")


def main():
    path = os.path.join(ROOT, "build/report.json")
    if not os.path.exists(path):
        sys.exit("build/report.json missing: run tools/bin/ninja report")
    r = json.load(open(path))
    row("all", r["measures"])
    for c in r.get("categories", []):
        row(c["id"], c["measures"])


if __name__ == "__main__":
    main()
