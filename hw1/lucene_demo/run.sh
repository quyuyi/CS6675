#!/bin/bash

# src: https://lucene.apache.org/core/8_7_0/demo/overview-summary.html#overview.description
# Extract 4 jar files from lucene-8.7.0 binary download

echo -n "Do you want to (re)build an index? Enter Y or n: "
read x
if [ "$x" = "Y" ]
then
    # Build an index
    java -cp ./lucene-analyzers-common-8.7.0.jar:./lucene-core-8.7.0.jar:./lucene-demo-8.7.0.jar:./lucene-queryparser-8.7.0.jar org.apache.lucene.demo.IndexFiles -docs cc.gatech.edu
fi

# Search the index
java -cp ./lucene-analyzers-common-8.7.0.jar:./lucene-core-8.7.0.jar:./lucene-demo-8.7.0.jar:./lucene-queryparser-8.7.0.jar org.apache.lucene.demo.SearchFiles
