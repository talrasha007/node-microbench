node-microbench
===============

A micro bench mark for node.js c++ addon.

# Usage
```
npm install -g matcha  # Install matcha first.
git clone https://github.com/talrasha007/node-microbench.git
cd node-microbench
npm install
matcha
```

# Conclusion
 - Performance of function call in C++ plugin is acceptable.
 - Using C++ plugin to deal with js object(s)/callback(s) is not so fast.
 - Creating Buffer object is slow.
  
# Result
```
// ENV: MBP with 16GB RAM
                      empty function call
   1,679,354,376 op/s » js
      38,450,717 op/s » c++

                      string ret function call
     169,870,780 op/s » js
      17,229,878 op/s » c++

                      get object properties
     159,686,245 op/s » js
       4,096,114 op/s » c++

                      set object properties & return
      36,974,504 op/s » js
       1,076,190 op/s » c++

                      object wrap
       2,529,886 op/s » ctor:js
       2,001,555 op/s » ctor:c++
      41,474,905 op/s » function call
      34,060,059 op/s » unwrap

                      callback
      51,469,968 op/s » js
       1,509,507 op/s » c++

                      buffer
       1,644,114 op/s » js
       1,621,187 op/s » c++
         735,819 op/s » c++:smalloc
```