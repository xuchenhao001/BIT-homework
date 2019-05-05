# ethro-data-fetch

# Config job

default config: 

```js
let url = "http://edu.ethro.net:9566/precedent/ElasticsearchController/search";
const codeTsMax = 9710;
const dirName = '../output/';
let codeTsStart = 0;
   
// multi thread download
let multiThreadsNum = 5;
```

> `url`: The address you want to fetch data from
>
> `codeTsMax`: The final element's codeTs you want to fetch. Default 9710
>
> `dirName`: The output directory to save data
>
> `codeTsStart`: From where to continue this fetch job. Notice that this will ignore the files already exists in your output dirctory. For example, if you set this as 0 and there are 0001-0005 codeTs files exist in your output directory, it will automatically detect all these files through (since this will cost several minutes), and then start from 0006.
>
> `multiThreadsNum`: the thread number of multi-thread data fetch, default is 5.

## Start job

```bash
npm install
./start.sh
```

logs output: `./fetch.log`

files output: `../output/`

## Sample logs output:

```
[2019-4-30 09:07:04] File successfully dumped: ../output/3153-000000000001
[2019-4-30 09:07:04] Total page number: 660.518
[2019-4-30 09:07:04] Current thread number: 1
[2019-4-30 09:07:05] File exists, ignore: ../output/3153-000000000002
[2019-4-30 09:07:05] Current thread number: 1
[2019-4-30 09:07:05] File exists, ignore: ../output/3153-000000000003
[2019-4-30 09:07:05] Current thread number: 1
[2019-4-30 09:07:05] File exists, ignore: ../output/3153-000000000004
[2019-4-30 09:07:05] Current thread number: 1
[2019-4-30 09:07:05] File exists, ignore: ../output/3153-000000000005
[2019-4-30 09:07:05] Current thread number: 1
[2019-4-30 09:07:05] File exists, ignore: ../output/3153-000000000006
[2019-4-30 09:07:05] Current thread number: 1
[2019-4-30 09:07:05] File exists, ignore: ../output/3153-000000000007
[2019-4-30 09:07:05] Current thread number: 1
[2019-4-30 09:07:05] File exists, ignore: ../output/3153-000000000008
[2019-4-30 09:07:05] Current thread number: 1
[2019-4-30 09:07:05] File exists, ignore: ../output/3153-000000000009
[2019-4-30 09:07:05] Current thread number: 1
[2019-4-30 09:07:05] File exists, ignore: ../output/3153-000000000010
[2019-4-30 09:07:05] Current thread number: 1
[2019-4-30 09:07:05] File exists, ignore: ../output/3153-000000000011
[2019-4-30 09:07:05] Current thread number: 1
[2019-4-30 09:07:05] File exists, ignore: ../output/3153-000000000012
[2019-4-30 09:07:05] Current thread number: 1
[2019-4-30 09:07:05] Current thread number: 2
[2019-4-30 09:07:05] File exists, ignore: ../output/3153-000000000014
[2019-4-30 09:07:05] Current thread number: 2
[2019-4-30 09:07:05] Current thread number: 3
[2019-4-30 09:07:05] Current thread number: 4
[2019-4-30 09:07:06] File exists, ignore: ../output/3153-000000000017
[2019-4-30 09:07:06] Current thread number: 4
[2019-4-30 09:07:06] File exists, ignore: ../output/3153-000000000018
[2019-4-30 09:07:06] Current thread number: 4
[2019-4-30 09:07:06] File exists, ignore: ../output/3153-000000000019
[2019-4-30 09:07:06] Current thread number: 4
[2019-4-30 09:07:06] Current thread number: 5
[2019-4-30 09:07:08] File successfully dumped: ../output/3153-000000000020
[2019-4-30 09:07:09] File successfully dumped: ../output/3153-000000000015
[2019-4-30 09:07:09] File successfully dumped: ../output/3153-000000000021
[2019-4-30 09:07:10] File successfully dumped: ../output/3153-000000000016
[2019-4-30 09:07:10] Current thread number: 2
[2019-4-30 09:07:10] Current thread number: 3
[2019-4-30 09:07:10] Current thread number: 4
[2019-4-30 09:07:11] Current thread number: 5
```