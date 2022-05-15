
import ErrorResponse from '../../../utils/errorResponse';
import { returnRespondToTheClientWithErr } from '../../../utils/sendResponse';

export default function errorHandler(err, req, res, next)
{    
    
    let error = { ...err };
    console.log(error);
    
    error.message = err.message;

    // Mongoose bad ObjectId
    if (err.name === 'CastError') {
        const message = `Resource not found`;
        error = new ErrorResponse(404, message);
    }

    // Mongoose duplicate key
    if (err.code === 11000) {
        const message = 'Duplicate field value entered';
        error = new ErrorResponse(400, message);
    }

    // Mongoose validation error
    if(err.code === 11000 || err.name === "ValidationError")
    {
        err.statusCode = 400;
    }

    returnRespondToTheClientWithErr(res, err.statusCode || 500, null, err.message || "server error");
}