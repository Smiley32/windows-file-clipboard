{
  "targets": [{
    "target_name": "clipboard",
    "include_dirs" : [
      "src",
      "<!(node -e \"require('nan')\")"
    ],
    "sources": [
      "src/clipboard.cc"
    ]
  }]
}