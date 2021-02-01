#!/bin/bash

# Use Apache Nutch to crawl websites
# https://cwiki.apache.org/confluence/display/NUTCH/NutchTutorial#NutchTutorial-Crawlyourfirstwebsite

# Download Nuch binary; cd apache-nutch-1.18/ as working directory

# 0. Create a URL seed list
mkdir -p urls
cd urls
touch seed.txt # type in the seed urls

# 1. Seed the crawldb with a list of URLs
bin/nutch inject crawl/crawldb urls

# 2. Fetch
# Generate a fetch list of all the pages due to be fetched
# Place the fetch list in a newly created segment directory
bin/nutch generate crawl/crawldb crawl/segments

# Run the fetcher on the segment
s1=`ls -d crawl/segments/2* | tail -1`
bin/nutch fetch $s1

# Parse the entries
bin/nutch parse $s1

# Update the databse with the results of the fetch
bin/nutch update crawl/crawldb $s1

# By repeating the above Fetch step, we can fetch more rounds.

# 3. Invertlinks
# So that we may index incoming anchor text with the pages
bin/nutch inverlinks crawl/linkdb -dir crawl/segments

# 4. Indexing into Apache Solr


# Read the crawldb, linkdb and segments.
# Read segment
bin/nutch readseg -dump ./crawl/segments/20210131192544/ result_seg/
# Read crawldb
bin/nutch readdb crawl/crawldb/ dump result_crawldb
# Read linkdb
bin/nutch readlinkdb crawl/linkdb/ -dump ./result_linkdb
