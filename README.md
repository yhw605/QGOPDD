# QGOPDD
A GNSS Observation&amp;Product Data Downloader based on Qt.
## The main idea
Well, I am a student major in Geodesy, and I am interested in GNSS. After trying some GNSS software tool suites, such as RTKLIB, Net_Diff, gLAB and CGO2, I think maybe create a program for downloading GNSS data is a good practice to polish my C/C++ programming skills and enhance my understanding of the subject.
## About
QGOPDD is a GNSS data downloader. It supports to download GNSS observation data through [IGS Data Center of Wuhan University](https://www.igs.gnsswhu.cn) with ftp client for now. Other GNSS data center will be supported later. 
For now, it just supports the download of observation data from a series of IGS stations in a particularly selected day, but does not support the download of data across days. This bug will be fixed soon.
## How to use
You can choose the stations you are interested in by checking the [IGS Network](https://network.igs.org) pre-loaded in this software. You can click "Go to" to know where this station exactly is located in, and after you find it, you can click "Add" to add this station to the table widget of waiting for downloading
