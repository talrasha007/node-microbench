{
  "targets": [
    {
      "target_name": "node-microbench",
	  "include_dirs" : [
        "<!(node -e \"require('nan')\")"
      ],
      "sources": [
        "src/main.cpp"
      ]
    }
  ]
}
