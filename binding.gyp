{
  "targets": [
    {
      "target_name": "node-microbench",
	  "include_dirs" : [
        "<!(node -e \"require('nan')\")",
        "<!(node -e \"require('nnu')\")"
      ],
      "sources": [
        "src/main.cpp"
      ]
    }
  ]
}
