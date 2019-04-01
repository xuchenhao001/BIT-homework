const rp = require('request-promise-native');
const fs = require('fs-extra');

let url = "http://edu.ethro.net:9566/precedent/ElasticsearchController/search";
const codeTsMax = 9710;
const dirName = '../output/';
let codeTsStart = 1340;

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
    logger("File successfully dumped: " + fileName);
    return response;
  } catch (error) {
    logger("Error: ", error);
    if (callback) {
      callback();
    }
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
    let multiThreadsNum = 5;
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
      logger("Total page number: " + totalPageNum)
    } else {
      logger("Error at codeTs: " + codeTsString);
      logger(res);
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
        logger("Current thread number: " + currentThreadsNum);

        fileName = dirName + codeTsString + '-' + paddy(pageNum, 12);
        let fileIsExist = fs.existsSync(fileName);
        if (fileIsExist && pageNum !== 1) {
          logger("File exists, ignore: " + fileName);
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
      logger("Wait for codeTs [" + codeTsString + "] finish...");
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

function logger(logcontent) {
  console.log("[" + (new Date()).toLocaleString() + "] " + logcontent);
}

processData();
