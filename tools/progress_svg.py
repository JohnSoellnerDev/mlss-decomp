#!/usr/bin/env python3
"""Draw the progress picture for the README from the objdiff report.

    tools/progress_svg.py [build/report.json] [docs/progress.svg]

The picture shows the share of code that is matched (split into complete units and matched
functions in the other units) and a map of all code units in ROM order, each as wide as its
code and shaded by how much of it is matched. Light and dark colours follow the viewer's
colour scheme.
"""
import datetime
import json
import os
import sys
from xml.sax.saxutils import escape

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

W, PAD = 840, 24
INNER = W - 2 * PAD
MAP_ROWS, ROW_H, ROW_GAP, GAP = 5, 18, 4, 2

# one blue ramp, light to dark (dark mode: dark to light); bins of "share matched"
BINS = [("0%", "zero"), ("under 25%", "b1"), ("under 50%", "b2"), ("under 75%", "b3"),
        ("under 100%", "b4"), ("complete", "b5")]
STYLE = """
.surface{fill:#fcfcfb;stroke:rgba(11,11,11,0.10)}
.t1{fill:#0b0b0b}.t2{fill:#52514e}.t3{fill:#898781}
.zero,.track{fill:#e1e0d9}.b1{fill:#86b6ef}.b2{fill:#5598e7}.b3{fill:#2a78d6}.b4{fill:#1c5cab}
.b5,.complete{fill:#104281}.matched{fill:#5598e7}
text{font-family:system-ui,-apple-system,"Segoe UI",sans-serif}
@media (prefers-color-scheme:dark){
.surface{fill:#1a1a19;stroke:rgba(255,255,255,0.10)}
.t1{fill:#f0efec}.t2{fill:#c3c2b7}.t3{fill:#898781}
.zero,.track{fill:#2c2c2a}.b1{fill:#184f95}.b2{fill:#256abf}.b3{fill:#3987e5}.b4{fill:#6da7ec}
.b5,.complete{fill:#b7d3f6}.matched{fill:#3987e5}
}
"""


def num(m, k):
    return float(m.get(k, 0) or 0)


def bin_of(unit):
    m = unit.get("measures", {})
    total, matched = num(m, "total_code"), num(m, "matched_code")
    if unit.get("metadata", {}).get("complete"):
        return "b5"
    share = matched / total if total else 0
    if share <= 0:
        return "zero"
    return "b1" if share < 0.25 else "b2" if share < 0.5 else "b3" if share < 0.75 else "b4"


def rect(x, y, w, h, cls, rx=2, title=None):
    t = f"<title>{escape(title)}</title>" if title else ""
    return f'<rect class="{cls}" x="{x:.1f}" y="{y:.1f}" width="{max(w, 0.5):.1f}" height="{h}" rx="{rx}">{t}</rect>'


def text(x, y, s, cls, size, weight=400, anchor="start"):
    return (f'<text class="{cls}" x="{x:.1f}" y="{y:.1f}" font-size="{size}" font-weight="{weight}" '
            f'text-anchor="{anchor}">{escape(s)}</text>')


def main(report_path, out_path):
    r = json.load(open(report_path))
    m = r["measures"]
    total = num(m, "total_code")
    matched_pct = num(m, "matched_code_percent")
    complete_pct = num(m, "complete_code_percent")
    partial_pct = max(matched_pct - complete_pct, 0)
    out = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="368" viewBox="0 0 {W} 368" '
           f'role="img" aria-label="Decompilation progress: {matched_pct:.1f}% of the code matched">',
           f"<style>{STYLE}</style>",
           '<rect class="surface" x="0.5" y="0.5" width="839" height="367" rx="12"/>',
           text(PAD, 38, "Mario & Luigi: Superstar Saga · matching decompilation", "t2", 14),
           f'<text x="{PAD}" y="88"><tspan class="t1" font-size="44" font-weight="600">{matched_pct:.1f}%</tspan>'
           f'<tspan class="t2" font-size="16" dx="10">of the code matched</tspan></text>',
           text(W - PAD, 62, f"{m['matched_functions']:,} of {m['total_functions']:,} functions", "t1", 14,
                anchor="end"),
           text(W - PAD, 84, f"{m.get('complete_units', 0)} of {m['total_units']} units complete", "t2", 14,
                anchor="end")]

    # bar: complete units, matched functions in other units, the rest
    y = 108
    wc, wp = INNER * complete_pct / 100, INNER * partial_pct / 100
    x = PAD
    if wc:
        out.append(rect(x, y, wc, 16, "complete", 4, f"complete units: {complete_pct:.1f}%"))
        x += wc + GAP
    if wp:
        out.append(rect(x, y, wp, 16, "matched", 4, f"matched in other units: {partial_pct:.1f}%"))
        x += wp + GAP
    out.append(rect(x, y, PAD + INNER - x, 16, "track", 4, "not matched yet"))
    lx = PAD
    for cls, label in (("complete", f"complete units {complete_pct:.1f}%"),
                       ("matched", f"matched in other units {partial_pct:.1f}%"),
                       ("track", "not matched yet")):
        out.append(rect(lx, 140, 10, 10, cls, 2))
        out.append(text(lx + 16, 149, label, "t2", 12))
        lx += 16 + 7 * len(label) + 28

    # map: every code unit in ROM order, as wide as its code, wrapped over MAP_ROWS rows
    units = [u for u in r["units"] if num(u.get("measures", {}), "total_code") > 0]
    out.append(text(PAD, 186, "Code units in ROM order (width = size of the code)", "t3", 12))
    top = 196
    usable = MAP_ROWS * INNER - GAP * len(units)
    pos = 0.0  # position along the wrapped strip
    for u in units:
        um = u["measures"]
        size = num(um, "total_code")
        w = usable * size / total
        share = num(um, "matched_code") / size * 100
        title = f"{u['name']}: {share:.0f}% matched ({um.get('matched_functions', 0)} of {um.get('total_functions', 0)} functions)"
        cls = bin_of(u)
        left = w
        while left > 0.01:
            row, col = divmod(pos, INNER)
            row = int(row)
            piece = min(left, INNER - col)
            out.append(rect(PAD + col, top + row * (ROW_H + ROW_GAP), piece, ROW_H, cls, 2, title))
            pos += piece
            left -= piece
        pos += GAP
    lx, ly = PAD, top + MAP_ROWS * (ROW_H + ROW_GAP) + 18
    for label, cls in BINS:
        out.append(rect(lx, ly - 9, 10, 10, cls, 2))
        out.append(text(lx + 16, ly, label, "t2", 12))
        lx += 16 + 7 * len(label) + 24
    day = datetime.date.today().isoformat()
    out.append(text(W - PAD, ly, f"updated {day}", "t3", 12, anchor="end"))
    out.append("</svg>")
    os.makedirs(os.path.dirname(out_path) or ".", exist_ok=True)
    with open(out_path, "w") as f:
        f.write("\n".join(out) + "\n")
    print(f"wrote {os.path.relpath(out_path, ROOT)}: {matched_pct:.2f}% matched, {complete_pct:.2f}% complete")


if __name__ == "__main__":
    a = sys.argv[1:]
    main(a[0] if a else os.path.join(ROOT, "build/report.json"),
         a[1] if len(a) > 1 else os.path.join(ROOT, "docs/progress.svg"))
