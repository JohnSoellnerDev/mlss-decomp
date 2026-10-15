#!/usr/bin/env python3
"""Draw the progress pictures for the README from the objdiff report, in the style decomp.dev
used: a treemap of all code units (area = size of the code; green = complete, blue = partly
matched, brighter the more is matched; dark gray = not started) and shields-style badges.

    tools/progress_svg.py [build/report.json] [OUTDIR]

Writes OUTDIR/progress.svg, OUTDIR/badge-code.svg, OUTDIR/badge-functions.svg and
OUTDIR/badge-complete.svg (OUTDIR defaults to build/progress). The Gitea workflow
(.gitea/workflows/progress.yml) runs this after every push and publishes the files on the
`progress` branch.
"""
import json
import os
import sys
from xml.sax.saxutils import escape

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
W, H = 950, 475
NONE, PARTIAL, DONE = (0x33, 0x33, 0x34), (0x00, 0x78, 0xB8), (0x00, 0xC0, 0x00)


def num(m, k):
    return float(m.get(k, 0) or 0)


def unit_color(m):
    total = num(m, "total_code")
    matched = num(m, "matched_code") / total if total else 0
    if matched >= 1:
        return DONE
    share = num(m, "fuzzy_match_percent") / 100 if "fuzzy_match_percent" in m else matched
    return tuple(round(a + (b - a) * share) for a, b in zip(NONE, PARTIAL))


def worst(row, length):
    s = sum(row)
    return max(length * length * max(row) / (s * s), (s * s) / (length * length * min(row)))


def squarify(items, x, y, w, h):
    """Squarified treemap (Bruls, Huizing, van Wijk): items are (area, data) sorted by area,
    largest first, with the areas adding up to w * h."""
    out = []
    while items:
        length = min(w, h)
        row = [items.pop(0)]
        while items and worst([a for a, _ in row + items[:1]], length) <= worst([a for a, _ in row], length):
            row.append(items.pop(0))
        s = sum(a for a, _ in row)
        if w >= h:
            rw = s / h if h else 0
            yy = y
            for a, d in row:
                rh = a / rw if rw else 0
                out.append((x, yy, rw, rh, d))
                yy += rh
            x, w = x + rw, w - rw
        else:
            rh = s / w if w else 0
            xx = x
            for a, d in row:
                rw = a / rh if rh else 0
                out.append((xx, y, rw, rh, d))
                xx += rw
            y, h = y + rh, h - rh
    return out


def treemap(report):
    units = [u for u in report["units"] if num(u.get("measures", {}), "total_code") > 0]
    total = sum(num(u["measures"], "total_code") for u in units)
    items = sorted(((num(u["measures"], "total_code") * W * H / total, u) for u in units),
                   key=lambda t: -t[0])
    m = report["measures"]
    out = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
           f'role="img" aria-label="Progress: {num(m, "matched_code_percent"):.2f}% of the code matched">',
           "<title>Code units, area = size of the code. Green: complete, blue: partly matched "
           "(brighter = more), gray: not started</title>",
           # the cushion: every block is shaded darker towards its edges
           '<defs><radialGradient id="c" cx="50%" cy="45%" r="70%">'
           '<stop offset="0" stop-color="#000" stop-opacity="0"/>'
           '<stop offset="1" stop-color="#000" stop-opacity="0.38"/></radialGradient></defs>',
           f'<rect width="{W}" height="{H}" fill="#1b1b1c"/>']
    for x, y, w, h, u in squarify(items, 0, 0, W, H):
        um = u["measures"]
        pct = num(um, "matched_code") / num(um, "total_code") * 100
        title = (f"{u['name']}: {pct:.1f}% matched, {um.get('matched_functions', 0)} of "
                 f"{um.get('total_functions', 0)} functions")
        r, g, b = unit_color(um)
        geo = f'x="{x:.2f}" y="{y:.2f}" width="{max(w, 0.2):.2f}" height="{max(h, 0.2):.2f}"'
        out.append(f'<g><title>{escape(title)}</title><rect {geo} fill="#{r:02x}{g:02x}{b:02x}"/>'
                   f'<rect {geo} fill="url(#c)" stroke="#000" stroke-opacity="0.55" stroke-width="1"/></g>')
    out.append("</svg>")
    return "\n".join(out) + "\n"


# shields.io "flat" badge, Verdana 11px widths (approximate per character)
WIDTH = {**{c: 7.0 for c in "0123456789"}, ".": 3.6, "%": 11.9, "/": 4.9, " ": 3.5, "i": 2.8,
         "l": 2.8, "f": 3.9, "t": 4.3, "r": 4.7, "m": 10.6, "w": 9.0, "C": 7.6, "F": 6.3}


def text_width(s):
    return sum(WIDTH.get(c, 7.5 if c.isupper() else 6.6) for c in s)


def badge(label, value, color="#007ec6"):
    lw, vw = round(text_width(label)) + 10, round(text_width(value)) + 10
    w = lw + vw

    def txt(x, s, width):
        return (f'<text aria-hidden="true" x="{x * 10}" y="150" fill="#010101" fill-opacity=".3" '
                f'transform="scale(.1)" textLength="{width * 10}">{escape(s)}</text>'
                f'<text x="{x * 10}" y="140" transform="scale(.1)" fill="#fff" '
                f'textLength="{width * 10}">{escape(s)}</text>')
    return (f'<svg xmlns="http://www.w3.org/2000/svg" width="{w}" height="20" role="img" '
            f'aria-label="{escape(label)}: {escape(value)}"><title>{escape(label)}: {escape(value)}</title>'
            '<linearGradient id="s" x2="0" y2="100%"><stop offset="0" stop-color="#bbb" stop-opacity=".1"/>'
            '<stop offset="1" stop-opacity=".1"/></linearGradient>'
            f'<clipPath id="r"><rect width="{w}" height="20" rx="3" fill="#fff"/></clipPath>'
            f'<g clip-path="url(#r)"><rect width="{lw}" height="20" fill="#555"/>'
            f'<rect x="{lw}" width="{vw}" height="20" fill="{color}"/>'
            f'<rect width="{w}" height="20" fill="url(#s)"/></g>'
            '<g fill="#fff" text-anchor="middle" font-family="Verdana,Geneva,DejaVu Sans,sans-serif" '
            'text-rendering="geometricPrecision" font-size="110">'
            f'{txt(lw / 2, label, lw - 10)}{txt(lw + vw / 2, value, vw - 10)}</g></svg>\n')


def main(report_path, outdir):
    report = json.load(open(report_path))
    m = report["measures"]
    os.makedirs(outdir, exist_ok=True)
    files = {
        "progress.svg": treemap(report),
        "badge-code.svg": badge("Code", f"{num(m, 'matched_code_percent'):.2f}%"),
        "badge-functions.svg": badge("Functions", f"{m['matched_functions']}/{m['total_functions']}"),
        "badge-complete.svg": badge("Complete", f"{num(m, 'complete_code_percent'):.2f}%"),
    }
    for name, text in files.items():
        with open(os.path.join(outdir, name), "w") as f:
            f.write(text)
    print(f"wrote {', '.join(files)} to {outdir}: {num(m, 'matched_code_percent'):.2f}% matched, "
          f"{num(m, 'complete_code_percent'):.2f}% complete")


if __name__ == "__main__":
    a = sys.argv[1:]
    main(a[0] if a else os.path.join(ROOT, "build/report.json"),
         a[1] if len(a) > 1 else os.path.join(ROOT, "build/progress"))
