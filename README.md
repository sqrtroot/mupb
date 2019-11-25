MuMp
----

![Mupb logo](https://raw.githubusercontent.com/sqrtroot/mupb/master/mupb.svg?sanitize=true "Logo")

# History

MuMp started as a modern c++ implementation of googles [protcol buffers](https://developers.google.com/protocol-buffers/) called MuPB.
Since the start of the project google released an update to their encoding scheme.
Which imo has one [big problem](https://developers.google.com/protocol-buffers/docs/encoding#order).
**Field orders aren't guaranteed anymore.**
[They used to be (snapshot from just before the change of feb 2019)](https://web.archive.org/web/20180224052609/https://developers.google.com/protocol-buffers/docs/encoding)
This removes the optimalisation oppertunity MuPB sougth to employ.
That's why MuPB is going to be it's own Message Packing Protocol and thus changing it's name from MuPB to MuMp.
Because of it's legacy it has a lot of simmilarities with Protocol Buffers.
