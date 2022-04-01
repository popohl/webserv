#!/bin/sh
#
# (c) 2008 BigSoft Limited
# Please retain this copyright message
# Software comes AS IS and without warrently
#
TITLE="Index Example"
echo "" > index.html
echo "$TITLE" >> index.html
echo "" >> index.html
echo "$TITLE" >> index.html
ls \
| grep -v index.html \
| sed 's/.*/& <\/a>/' \
>> index.html
