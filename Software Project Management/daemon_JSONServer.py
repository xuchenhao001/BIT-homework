# -*- coding:utf-8 -*-

import daemon
from SimpleJSONServer import main

with daemon.DaemonContext():
    main()
