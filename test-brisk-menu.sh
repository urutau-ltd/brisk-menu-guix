#!/usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail

killall -q brisk-menu

mate-panel-test-applets --iid BriskMenuFactory::BriskMenuApplet
