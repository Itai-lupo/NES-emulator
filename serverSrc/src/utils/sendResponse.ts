export function returnSuccessRespondToTheClient(res, status, data)
{
    return res.status(status).json({
        success: true,
        data
    });
}

export function returnRespondToTheClientWithErr(res, status, data, error)
{
    return res.status(status).json({
        success: false,
        data,
        error
    });
}


export function returnSuccessRespondToTheClientWithPage(res, status, data, pagination)
{
    return res.status(status).json({
        success: true,
        data,
        pagination
    });
}