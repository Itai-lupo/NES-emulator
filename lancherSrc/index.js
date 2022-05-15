const { app, BrowserWindow, contextBridge, ipcMain } = require("electron");
const path = require('path')
const url = require('url')
const fs = require('fs');
const axios = require('axios')


ipc = ipcMain;


app.on("ready", () => {
  app.name = 'lancher';
  const mainWindow = new BrowserWindow({
    title: 'lancher',
    webPreferences: {
        nodeIntegration: true,
        contextIsolation: false,
        enableRemoteModule: true
    }
});
  mainWindow.loadFile(path.join(__dirname, "public/index.html"));
});


app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') app.quit()
})


ipcMain.on('lanchGame', async (event, args) => {
  var executablePath;
  if (process.platform == "linux") 
    executablePath = "./run.sh";
  else if(process.platform == "win32")
    executablePath = "./run.bat";
    
  let child = require('child_process').execFile;
  var parameters = [args];
  console.log(args)
  child(executablePath, [args], function(err, data) {
      console.log(err)
      console.log(data.toString());
  });
  
  event.returnValue = "sucsses";

});


ipcMain.on('downloadGame', async (event, args) => {

  const res = await axios.get(args.server.host + '/api/v1/downloadGame/' + args.gameName, 
      {
        responseType: 'arraybuffer',
        headers: {
        authorization: "Bearer " + args.server.token,
      }
    });
  console.log(res.data.length);
  console.log(res.headers['content-length']);

  fs.writeFileSync("./res/roms/" + args.gameName + ".nes", res.data, "binary", res.headers['content-length'], 0 )
  event.returnValue = "download";
});

ipcMain.on('getGameList', (event, message) => 
{
  let paths = []; 
  fs.readdirSync("./res/roms/").forEach(file => {
    paths.push(file);
  }); 
  paths = paths.map((c) => {  return c.split(".")[0] })
  // console.log(paths);
  event.returnValue = paths;
})

