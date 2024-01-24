### WhereIsDep


####  C++ CLI application to search package.json dependencies in given directories.


<img src="./whereisdep.png" width="600">


##### Run:
```bash
make run
```

##### [for development] Compile and run:
```bash
make buildandrun
```



#### CLI options:


| option                       | description                                          | is required                                                  | default value      |
| :---                         | :---                                                 | :---                                                         | :---               |
| -p \| --packageJsonFilePath  | Set the path of the package.json file                | <small><span style="color:red;">(* required)</span></small>  | N/A                |
| -s \| --searchDir            | Set the searching directory that contains JS files   | <small><span style="color:red;">(* required)</span></small>  | N/A                |
| -e \| -ext                   | Set the target search files extensions               | <small><span style="color:gray;">(optional)</span></small>   | [".js", ".jsx"]    |



  
  
  
#### Add executable file to your bin folder
To use whereisdep globally from any terminal window `$ whereisdep` :
- Mac/Linux: `cp whereisdep /usr/local/bin/whereisdep`
  
  