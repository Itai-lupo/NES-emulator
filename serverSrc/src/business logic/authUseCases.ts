import ErrorResponse from "../utils/errorResponse";
import jwt from 'jsonwebtoken';

import IDBManger from "../IO_Mangers/DBManger/intrface/IDBManger";

import {
    returnSuccessRespondToTheClient,
    returnRespondToTheClientWithErr,
    returnSuccessRespondToTheClientWithPage
} from '../utils/sendResponse';


export default class authLogic
{
    private db:IDBManger;

    constructor(db)
    {
        this.db = db;
    }

    login = async (req, res, next) => {
        const { email, password } = req.body;
        
        if(!email || !password)
            return next(new ErrorResponse(400, 'please provide an email and a password'));

        const user = await this.db.findOne({email}, {select: '+password'});
        
        if(!user)
            return next(new ErrorResponse(401, `invalid credentials`));

        const isMatch = await user.matchPassword(password);

        if(!isMatch)
            return next(new ErrorResponse(401, `invalid credentials`));

        
        const token = user.getSignedJwtToken();
        res.set("resType", "login");

        returnSuccessRespondToTheClient(res, 200, token);
    }

    signup = async (req, res, next) => {
        const { email, name, password, serverKey } = req.body;
        

        if(serverKey != process.env.SERVER_KEY)
            return next(new ErrorResponse(401, 'please provide an email and a password'));

        if(!email || !password)
            return next(new ErrorResponse(400, 'please provide an email and a password'));

        const user = await this.db.create({email, name, password});
        
        
        const token = user.getSignedJwtToken();

        returnSuccessRespondToTheClient(res, 200, token);
    }

    protect = async (req, res, next) => {
        let token;
        
        if(req.headers.authorization && req.headers.authorization.startsWith('Bearer'))
            token = req.headers.authorization.split(' ')[1];

        if(!token)
            return next(new ErrorResponse(401, 'not authorize to acccess this route'));
    
        try {
            
            const decode = jwt.verify(token, process.env.JWT_SECRET);
            req.user = await this.db.getSingleById(decode.id);
    
            next();
        } catch (error) {
            return next(new ErrorResponse(401, 'not authorize to acccess this route'));
        }
    }

    authorize = (...roles):any => {
        return (req, res, next) => {
            if(!roles.includes(req.user.role))
                return next(new ErrorResponse(403, `User role is not authorized to access this route `));
            next();
        }
    }
}