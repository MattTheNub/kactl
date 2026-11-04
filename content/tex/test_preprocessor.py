"""Run from the repository root: python3 -m unittest discover -s content/tex."""
import io
import re
import subprocess
import unittest
from unittest.mock import patch

import preprocessor

BADGE = r' \(\*@\\kactlcheckpoint\{([0-9a-f]{6})\}@\*\)'


def digest(source):
    return subprocess.run(
        ['sh', 'content/contest/hash.sh'], input=source, text=True,
        capture_output=True, check=True).stdout.strip()


class PrefixCheckpointsTest(unittest.TestCase):
    def checkpoints(self, source):
        rendered = preprocessor.prefix_checkpoints(source)
        self.assertEqual(len(source.splitlines()), len(rendered.splitlines()))
        self.assertEqual(digest(source), digest(re.sub(BADGE, '', rendered)))
        original = source.splitlines()
        checkpoints = []
        for i, line in enumerate(rendered.splitlines()):
            match = re.search(BADGE + '$', line)
            if match:
                self.assertEqual(line[:match.start()], original[i])
                self.assertLessEqual(len(line[:match.start()].expandtabs(2)) + 11, 63)
                self.assertEqual(match[1], digest('\n'.join(original[:i + 1])))
                checkpoints.append((i, match[1]))
            else:
                self.assertEqual(line, original[i])
        return checkpoints

    def test_frequency_and_spacing(self):
        for n in (7, 8, 10, 15, 29, 30, 39, 40, 60, 90, 180):
            with self.subTest(lines=n):
                source = '\n'.join('int x%d = %d;' % (i, i) for i in range(n))
                points = self.checkpoints(source)
                self.assertEqual(len(points), len(range(4, n - 3, 5)) + 1)
                self.assertEqual(points[-1], (n - 1, digest(source)))
                boundaries = [-1] + [i for i, _ in points]
                self.assertTrue(all(b - a >= 3 for a, b in zip(boundaries, boundaries[1:])))

    def test_reflow_and_localization(self):
        source = '\n'.join('int x%d = %d;' % (i, i) for i in range(90))
        points = self.checkpoints(source)
        for i, expected in points:
            prefix = '\n'.join(source.splitlines()[:i + 1])
            self.assertEqual(digest(prefix.replace(';', ';\n\n').replace(' = ', '\t=\t')), expected)
        changed = source.splitlines()
        changed[points[0][0] + 1] = 'int typo = 0;'
        self.assertEqual([digest('\n'.join(changed[:i + 1])) == h for i, h in points],
                         [True] + [False] * (len(points) - 1))

    def test_unsafe_endpoints(self):
        for opening, closing in [('/*', '*/'), ('auto s = R"tag(', ')tag";')]:
            with self.subTest(opening=opening):
                source = '\n'.join(['int a;'] * 5 + [opening] + ['x;'] * 20 + [closing] + ['int b;'] * 5)
                self.assertTrue(all(not 5 <= i <= 25 for i, _ in self.checkpoints(source)))
        for opening in ['#define F \\', '// comment \\']:
            source = '\n'.join(['int a;'] * 5 + [opening] + ['x; \\'] * 19 + ['x;'] + ['int b;'] * 6)
            self.assertTrue(all(not 5 <= i <= 25 for i, _ in self.checkpoints(source)))

    def test_crowded_lines(self):
        source = '\n'.join('int ' + 'x' * 48 + ';' for _ in range(40))
        self.assertEqual(self.checkpoints(source), [])

    def test_final_hash_on_short_listings(self):
        for source in ('int x;', 'void f() {\n}', 'struct X {\n\tint x;\n};'):
            with self.subTest(source=source):
                self.assertEqual(self.checkpoints(source),
                                 [(len(source.splitlines()) - 1, digest(source))])

    def test_final_hash_width_limit(self):
        for width in (52, 53):
            with self.subTest(width=width):
                source = '\n'.join(['int x;'] * 10 + ['int ' + 'x' * (width - 5) + ';'])
                points = self.checkpoints(source)
                self.assertEqual(any(i == 10 for i, _ in points), width == 52)

    def test_unsafe_final_line(self):
        for source in ('', '// comment;', '#define X 1;', '/*\nint x;',
                       'auto s = R"tag(\nint x;', '#define X \\\nint x;'):
            with self.subTest(source=source):
                self.assertNotIn(r'\kactlcheckpoint{', preprocessor.prefix_checkpoints(source))

    def test_listing_integration(self):
        with open('content/graph/MinCostMaxFlow.h') as f:
            source = f.read()
        outputs = []
        with patch.object(preprocessor, 'addref'):
            for annotate in (False, True):
                output = io.StringIO()
                if annotate:
                    preprocessor.processwithcomments('MCMF.h', io.StringIO(source), output, 'C++')
                else:
                    with patch.object(preprocessor, 'prefix_checkpoints', side_effect=lambda s: s):
                        preprocessor.processwithcomments('MCMF.h', io.StringIO(source), output, 'C++')
                outputs.append(output.getvalue())
        self.assertIn(r'\kactlcheckpoint{', outputs[1])
        self.assertEqual(re.sub(BADGE, '', outputs[1]), outputs[0])
        heading_hash = re.search(r'\\rightcaption\{([0-9a-f]{6}),', outputs[1])[1]
        self.assertRegex(outputs[1], BADGE.replace('([0-9a-f]{6})', heading_hash)
                         + r'\n\\end\{lstlisting\}')


if __name__ == '__main__':
    unittest.main()
