#!/bin/bash

export DOCS_PATH=/home/manel/CLionProjects/dprot/dprot_pract2_5/docs
export NODES_PATH=/home/manel/CLionProjects/dprot/dprot_pract2_5/nodes

cat $DOCS_PATH/doc.pre $DOCS_PATH/doc$1.dat | openssl dgst -sha1 -binary > $NODES_PATH/node0.$1