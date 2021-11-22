#!/bin/bash

export NODES_PATH=/home/manel/CLionProjects/dprot/dprot_pract2_5/nodes

cat $NODES_PATH/node.pre $NODES_PATH/node$1.$3 $NODES_PATH/node$1.$4 | openssl dgst -sha1 -binary > $NODES_PATH/node$2.$5