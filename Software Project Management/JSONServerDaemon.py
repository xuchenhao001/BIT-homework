# -*- coding:utf-8 -*-

import daemon
from JSONServer import main

with daemon.DaemonContext():
    main()
