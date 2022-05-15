export default class ErrorResponse extends Error 
{
    private statusCode;
    constructor(statusCode:number, msg:string)
    {
        super(msg);

        this.statusCode = statusCode;

        Error.captureStackTrace(this, this.constructor);
    }
}
