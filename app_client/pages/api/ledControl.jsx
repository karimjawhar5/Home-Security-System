export default async function ledControl(req, res){

     const {state} = req.body;
    try {
        const {status, message} = await postData('http://192.168.2.140/LED', {state:state})
        if(status == "error"){
            console.log(status)
            console.log(message)
            res.status(500).send("Server Error")
        }else{
            res.status(200).json({status:status, message:message})
        }

    }catch{
        res.status(404).send("failed to connect to ESP")
        console.log("failed to connect to ESP")
    }
}

async function postData(url, data) {
    const response = await fetch(url, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(data)
    });
    return response.json();
  }