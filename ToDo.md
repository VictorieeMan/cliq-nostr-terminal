To do and the date it was added

2023-07-18
- [ ] Get Nostr data //The nostr protocol is based on websocket communication, sending and receiving json formatted data.
  - [x] install a webbsocket library for c++ //Decided on [libwebsockets](https://libwebsockets.org/) version [4.3.2](https://github.com/warmcat/libwebsockets/releases/tag/v4.3.2)
  - [x] Install a Json library; choosen simdjson for its speed.
  - [ ] Fetch this nostr message:
```
nkey: nevent1qqsy8hark96tc7j7s9zazt3e8035zpl26wdp2e233nf2g6g7s4tn22c9qz29u
hex: 43dfa3b174bc7a5e8145d12e393be34107ead39a1565518cd2a4691e8557352b
```