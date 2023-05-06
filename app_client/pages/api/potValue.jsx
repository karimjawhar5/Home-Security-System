
export default async function getPotValue(req, res){
    try{
        const response = await fetch('http://192.168.2.140/POT')
        const {status, message} = await response.json()

        if(status == "success"){
            res.status(200).json({value: message})
        }else{
            res.status(500).send(message)
        }

    }catch{
        res.status(404).send("failed to connect to ESP")
        console.log("failed to connect to ESP")
    }
}