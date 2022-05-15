import IAPIManager from "./IAPIManager";
import express = require("express");
import https from "https";
import http from "http";

import cors from 'cors';
import fs from 'fs'
import logger from "./middleware/logger";
import errorHandler from "./middleware/error";

 
export default class expressApi implements IAPIManager
{

	private server: https.Server;
	private app;

    constructor(port: number)
    {
        this.app = express();

        this.server = https.createServer({
            key: fs.readFileSync("key.pem"),
            cert: fs.readFileSync("cert.pem"),
          }, this.app)

        this.server.listen(
            port,
            () => console.log(`Server running in ${process.env.NODE_ENV} mode on port ${port}`)
        );

        this.app.listen(
            port + 1,
            () => console.log(`Server running in ${process.env.NODE_ENV} mode on port ${port + 1}`)
        );

        this.initMiddlewareBefore();

        // this.server = server;
    }

    getApp() {
        return this.app;
    }

    private initMiddlewareBefore(): void
    {
        this.app.use(express.json());
        this.app.use(express.static('public'));
        console.log(process.env.NODE_ENV)
        if(process.env.NODE_ENV === 'development') this.app.use(logger);
        
        this.app.use(cors());
        
    }

    public initMiddleware(): void
    {
        this.app.use(errorHandler);
        
    }

    public addRoute(method:string, path: string, functionToCall: (req: any, res: any, next: any) => void )
    {
        (this.app as any)[method](path, functionToCall);
    }

    close(): void {
        this.server.close();
    }
}
