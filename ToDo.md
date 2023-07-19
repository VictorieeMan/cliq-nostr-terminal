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

```
{
  "content": "Nostr is springing to life again 🫂",
  "created_at": 1688232266,
  "id": "43dfa3b174bc7a5e8145d12e393be34107ead39a1565518cd2a4691e8557352b",
  "kind": 1,
  "pubkey": "82341f882b6eabcd2ba7f1ef90aad961cf074af15b9ef44a09f9d2a8fbfbe6a2",
  "sig": "2b0c6519de67b7093ec5d0b8d77e30db624d36b529e81cb012294b5ad5603f5e17c8fb221f3603819619145a0e5ba9287b8dfbffd5d3e56d134911691e74e85d",
  "tags": [],
  "seenOn": [
    "wss://nostr.wine/"
  ]
}
```