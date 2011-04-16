TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = qcc-core \
          qcc-server \
          qcc

qcc-server.depends = qcc-core
qcc.depends = qcc-core

OTHER_FILES += README.md \
               COPYING
