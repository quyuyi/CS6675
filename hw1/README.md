# CS6675 Homework1: Web Crawler

## How to Run
### Running Apache Nutch to Crawl [\[Reference\]](https://cwiki.apache.org/confluence/display/NUTCH/NutchTutorial)
First download Apache Nutch (binary version) from [here](http://nutch.apache.org/). Make `apache-nutch-1.18/` as the working directory.

Create a url seed list:
```bash
mkdir -p urls
cd urls
touch seed.txt # type in the seed urls in this file
```

Seed the `crawldb` with a list of URLs. A `crawldb` contains information about every URL known to Nutch, including status (whether it was fetiched), fetching time, score, signature, metadata, etc.
```bash
bin/nutch inject crawl/crawldb urls
```

Then generate a fetch list from the crawldb. The list contains the URLs that are due to be fetched, either unfetched or need re-fetch. A new directory is created for this segment in the `segments` directory, using the current timestamp as the directory name. The list is stored in `crawl_generate` in the segment directory.
```bash
bin/nutch generate crawl/crawldb crawl/segments
```

We can then fetch on this segment. Fetched reults are stored in `content` and `crawl_fetch` in this segment directory. `content` contains the raw content retrieved from each URL. `crawl_fetch` contains the status of fetching each URL.
```bash
s1=`ls -d crawl/segments/2* | tail -1` # path to the newest segment
bin/nutch fetch $s1
```

We can parse the fetched results. This will generate `parse_data`, `parse_text`, and  `crawl_parse` in this segment directory. `parse_data` contains outlinks and metadata parsed from each URL. `parse_text` contains the parsed text of each URL. `crawl_parse` contains the outlink URLs, used to update the crawldb.
```bash
bin/nutch parse $s1
```

Then we can update the crawldb, marking some URLs as fetched, and also inserting the new URLs extracted from the pages that are fetched in this round.
```bash
bin/nutch updatedb crawl/crawldb $s1
```

This is one round of fetching. We can repeat this to fetch multiple rounds to crawl more pages.

We can create the `linkdb`. The `linkdb` contains the list of known links to each URL, including both the source URL and anchor text of the link (the clickable text on the source page that leads to this page).
```bash
bin/nutch inverlinks crawl/linkdb -dir crawl/segments
```

We can inspect the `crawldb`, `linkdb` and the information for each `segment` with the following commands:
```bash
bin/nutch readdb crawl/crawldb/ -dump <out_dir>/ # Read crawldb
bin/nutch readlinkdb crawl/linkdb/ -dump <out_dir>/ # Read linkdb
bin/nutch readseg -dump ./crawl/segments/<segment_dir_name>/ <out_dir>/ # Read segment
```
Example storage in `crawldb`:
```bash
https://issuu.com/gt-computing	Version: 7
Status: 1 (db_unfetched)
Fetch time: Sun Jan 31 19:47:32 EST 2021
Modified time: Wed Dec 31 19:00:00 EST 1969
Retries since fetch: 0
Retry interval: 2592000 seconds (30 days)
Score: 3.8461538E-4
Signature: null
Metadata: 
```
Example storage in `linkdb`:
```bash
http://career.gatech.edu/contact	Inlinks:
 fromUrl: https://www.cc.gatech.edu/content/internships-and-co-ops anchor: Contact the GTIP Team
 fromUrl: https://www.cc.gatech.edu/content/internships-and-co-ops anchor: Contact the Undergrad Co-op Team
 fromUrl: https://www.cc.gatech.edu/content/internships-and-co-ops anchor: Contact the Graduate Co-op Team
```

### Running Apache Solr to Build Index
First download Apache Solr (binary version) from [here](https://www.apache.org/dyn/closer.cgi/lucene/solr/). Integrate Apache Solr with Apache Nutch. First create new resources for a new "nutch" Solr core. Copy the Nutch's [schema.xml](https://raw.githubusercontent.com/apache/nutch/master/src/plugin/indexer-solr/schema.xml) into the Solr `conf` directory. Further details can be found [here](https://cwiki.apache.org/confluence/display/NUTCH/NutchTutorial#NutchTutorial-SetupSolrforsearch). 

Start the Solr server and create the nutch core
```bash
${APACHE_SOLR_HOME}/bin/solr start
${APACHE_SOLR_HOME}/bin/solr create -c nutch -d ${APACHE_SOLR_HOME}/server/solr/configsets/nutch/conf/
```

Then we can index the segments into Apache Solr.
```bash
bin/nutch index crawl/crawldb/ -linkdb crawl/linkdb/ crawl/segments/<segment>/ -filter -normalize -deleteGone
```

Navigate to http://localhost:8983/solr/ and select the nutch core, where there is a GUI to do search query and view the search results.

## Experiment
I seed the crawler with one single seed list https://www.cc.gatech.edu/. I crawled ~7000 pages, extracting ~50000 pages from them in 4 hours, doing 5 rounds of nutch fetch. The `crawl/` directory contains the results generated during my experiment.