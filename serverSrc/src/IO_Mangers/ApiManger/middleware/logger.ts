export default function logger(req, res, next):any
{
    console.log(`${req.method} ${req.protocol}://${req.get("host")}${req.url}`);
    
    next();
}