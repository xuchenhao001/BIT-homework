# -*- coding:utf-8 -*-

import daemon
from HTTPServer import main

with daemon.DaemonContext():
    main()
