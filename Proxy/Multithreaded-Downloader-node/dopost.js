const rp = require('request-promise-native');
const fs = require('fs-extra');

let url = "http://edu.ethro.net:9566/precedent/ElasticsearchController/search";
const codeTsMax = 9710;
const dirName = '../output/';
let codeTsStart = 416;

async function doPost(codeTS, pageNum, fileName, callback) {
  let payload = {
    pageNum: pageNum,
    pageSize: "500",
    publicCaseId: "",
    goodsName: "",
    goodsModel: "",
    codeTs: codeTS
  };

  try {
    let response = await rp({
      url: url,
      method: "POST",
      json: true,
      headers: {
        "content-type": "application/json",
      },
      body: payload
    });
    if (callback) {
      callback();
    }
    fs.outputFileSync(fileName, JSON.stringify(response));
    console.log("file successfully dumped: " + fileName);
    return response;
  } catch (error) {
    console.log("Error: ", error);
    return error;
  }
}

async function processData() {

  for (let codeTs = codeTsStart; codeTs <= codeTsMax; codeTs++) {
    let total = 1;
    let codeTsString = paddy(codeTs, 4);
    let pageNum = 1;
    let totalPageNum = 1;

    // multi thread download
    let multiThreadsNum = 10;
    let currentThreadsNum = 0;
    let currentThreadsDD = function () {
      currentThreadsNum--;
    };

    // init total page number with first post
    let fileName = dirName + codeTsString + '-' + paddy(pageNum, 12);
    let res = await doPost(codeTsString, "" + pageNum, fileName);
    if (res.status === 200) {
      total = res.data.total;
      totalPageNum = total / 500;
      console.log('Total page number: ' + totalPageNum)
    } else {
      console.log('Error at codeTs: ' + codeTsString);
      console.log(res);
      codeTs--;
      continue;
    }

    // synchronized download rest files
    while (pageNum <= totalPageNum + 1) {
      if (currentThreadsNum < multiThreadsNum) {

        // if this codeTs is full, jump out loop
        if (pageNum > totalPageNum + 1) {
          break;
        }
        pageNum++;
        currentThreadsNum++;
        console.log('current thread number: ' + currentThreadsNum);

        fileName = dirName + codeTsString + '-' + paddy(pageNum, 12);
        let fileIsExist = fs.existsSync(fileName);
        if (fileIsExist && pageNum !== 1) {
          console.log("file exists, ignore: " + fileName);
          currentThreadsNum--;
          continue;
        }
        doPost(codeTsString, "" + pageNum, fileName, currentThreadsDD);
      }
      if (currentThreadsNum === multiThreadsNum) {
        await sleep(3000);
      }
    }

    // wait until this codeTs finished
    while (currentThreadsNum > 0) {
      console.log('Wait for codeTs [' + codeTsString + '] finish...');
      await sleep(3000);
    }

  }
}

let sleep = async function (ms) {
  return new Promise(resolve => {
    setTimeout(resolve, ms)
  })
};


function paddy(num, padlen, padchar) {
  let pad_char = typeof padchar !== 'undefined' ? padchar : '0';
  let pad = new Array(1 + padlen).join(pad_char);
  return (pad + num).slice(-pad.length);
}

processData();
