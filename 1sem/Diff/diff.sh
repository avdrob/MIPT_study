#!/bin/bash
`gedit diff.txt`
`./diff`
`latex diff.tex`
`dvipdfm diff.dvi`
`wait`
`rm diff.dvi`
`rm diff.log`
`rm diff.aux`
`atril ./diff.pdf`



