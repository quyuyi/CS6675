#!/bin/bash

# Use Apache Nutch to crawl websites
# https://cwiki.apache.org/confluence/display/NUTCH/NutchTutorial#NutchTutorial-Crawlyourfirstwebsite

# Download Nuch binary; cd apache-nutch-1.18/ as working directory

# 0. Create a URL seed list
mkdir -p urls2
cd urls2
touch seed.txt # type in the seed urls

echo "Type in the seed urls in urls/seed.txt. Press Enter if finished."
read x

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
bin/nutch updatedb crawl/crawldb $s1

# By repeating the above Fetch step, we can fetch more rounds.

# 3. Invertlinks
# So that we may index incoming anchor text with the pages
bin/nutch inverlinks crawl/linkdb -dir crawl/segments

# 4. Indexing into Apache Solr
bin/nutch index crawl/crawldb/ -linkdb crawl/linkdb/ crawl/segments/* -filter -normalize -deleteGone

# Read the crawldb, linkdb and segments.
# Read crawldb
bin/nutch readdb crawl/crawldb/ -dump <out_dir>
# Read segment
bin/nutch readseg -dump ./crawl/segments/<segment>/ <out_dir>
# Read linkdb
bin/nutch readlinkdb crawl/linkdb/ -dump <out_dir>