#!/bin/bash

set -x

nohup node dopost.js > ./fetch.log 2>&1 &

