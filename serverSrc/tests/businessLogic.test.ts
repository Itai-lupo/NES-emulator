require("../src/utils/dotenvInit");

import jwt from 'jsonwebtoken';

import IValidetor from 'validetors/IValidetor';
import IDBManger from '../src/IO_Mangers/DBManger/intrface/IDBManger';

import mongoose = require('mongoose');
import { MongoMemoryServer } from "mongodb-memory-server";


import AuthDBManger from 'IO_Mangers/DBManger/mongoDB/AuthDBManger';



import ConcreteMediators from 'Mediator/ConcreteMediators';
import ErrorResponse from 'utils/errorResponse';


let userDB:IDBManger;

let mongoServer: MongoMemoryServer;

beforeEach(async () => {
	mongoServer = new MongoMemoryServer();

    
	const mongoUri = await mongoServer.getUri();


    userDB = new AuthDBManger();

    await userDB.connect(mongoUri);

});


afterEach(() => {
    return mongoose.disconnect();
})

test("business logic", async () => {})
