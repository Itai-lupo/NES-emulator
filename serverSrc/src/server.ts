require("./utils/dotenvInit");

import IAPIManagers from "./IO_Mangers/ApiManger/IAPIManager";

import { createDBManger, createAPIManger } from "./utils/MangersInit";

import authLogic from "./business logic/authUseCases";
import gameDownloadLogic from "./business logic/gameDownloadUseCases";

import ConcreteMediators from "./Mediator/ConcreteMediators";
import IValidetor from "./validetors/IValidetor";

import nodeCleanup from "node-cleanup";

const PORT: number = parseInt(process.env.PORT  || '4000');
const APIManger:IAPIManagers = createAPIManger(PORT);

const {usersDBManger} = createDBManger();

 



usersDBManger.connect(process.env.MONGO_URI);

const authManger:authLogic = new authLogic(usersDBManger);
const gamesManger:gameDownloadLogic = new gameDownloadLogic();

initIOInputRoutes();

process.on("unhandledRejection", (err: any, promise) => {
    if(!err.message) err.message = "server error";
    console.log(`error: ${err}`);
})

process.on('exit', () => {
    console.log("exiting");
    
    APIManger.close();
})

let stopping = false;
nodeCleanup((exitCode, signal) => {
  if (!stopping) {
    stopping = true;
    process.exit();
  }
  return false;
});

process.on("SIGUSR2", () => process.kill(process.pid, "SIGHUP"));

function initIOInputRoutes()
{

    const routes = [
            {
                method: 'post',
                path: '/api/v1/login',
                callback: authManger.login
            },
            {
                method: 'put',
                path: '/api/v1/signup',
                callback: authManger.signup
            },
            {
                method: 'get',
                path: '/api/v1/downloadGame/:gameName',
                callback: [ authManger.protect, gamesManger.downloadGame]
            },
            {
                method: 'get',
                path: '/api/v1/getGameList',
                callback: [ gamesManger.gameList]
            }
        ];

    routes.forEach(route => {
        APIManger.addRoute(route.method, route.path, route.callback);
    })
    APIManger.initMiddleware();

}