# CS6675 Homework1: Web Crawler

## Web archiving
- The process of collecting portions of the World Wide Web to ensure the information is preserved in an archive for future researchers, hostorians, and the public.

## Apache Nutch
Nutch data is composed of:
- Crawldb \
Contains information about every URL known to Nutch, including status (whether it was fetiched), fetching time, score, signature, metadata, etc.
```
https://issuu.com/gt-computing	Version: 7
Status: 1 (db_unfetched)
Fetch time: Sun Jan 31 19:47:32 EST 2021
Modified time: Wed Dec 31 19:00:00 EST 1969
Retries since fetch: 0
Retry interval: 2592000 seconds (30 days)
Score: 3.8461538E-4
Signature: null
Metadata: 

https://issuu.com/gt-computing/docs/12001_collegeofcomputing_impactreport_web_spreads	Version: 7
Status: 2 (db_fetched)
Fetch time: Tue Mar 02 19:26:45 EST 2021
Modified time: Sun Jan 31 19:26:45 EST 2021
Retries since fetch: 0
Retry interval: 2592000 seconds (30 days)
Score: 0.016699996
Signature: fafd8b62bbeeffeb0aa6a3bee1adbb85
Metadata: 
 	_pst_=success(1), lastModified=1612139204000
	_rs_=372
	Content-Type=text/html
	nutch.protocol.code=200
```

- Linkdb \
Contains the list of known links to each URL, including both the source URL and anchor text of the link (the clickable text on the source page that leads to this page).
```
http://career.gatech.edu/	Inlinks:
 fromUrl: https://news.gatech.edu/features/conversations-cabrera-charles-isbell anchor: Career Center

http://career.gatech.edu/contact	Inlinks:
 fromUrl: https://www.cc.gatech.edu/content/internships-and-co-ops anchor: Contact the GTIP Team
 fromUrl: https://www.cc.gatech.edu/content/internships-and-co-ops anchor: Contact the Undergrad Co-op Team
 fromUrl: https://www.cc.gatech.edu/content/internships-and-co-ops anchor: Contact the Graduate Co-op Team
```

- A set of segments. Each segment is a set of URLs that are fetched as a unit. Segments are directories with the following subdirectories:
    - a *crawl_generate* names a set of URLs to be fetched
    - a *crawl_fetch* contains the status of fetching each URL
    - a *content* contains the raw content retrieved from each URL
    - a *parse_text* contains the parsed text of each URL
    - a *parse_data* contains outlinks and metadata parsed from each URL
    - a *parse_crawl* contains the outlink URLs, used to update the crawldb