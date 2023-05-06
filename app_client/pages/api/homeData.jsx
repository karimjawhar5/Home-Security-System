
export default async function homeData(req, res){
    try{
        const response = await fetch('http://192.168.2.140/DATA');
        const {status, led, pot} = await response.json()
        if(status == "success"){
            res.status(200).json({led: led, pot: pot})
        }else{
            res.status(500).send("Internal Server Error")
        }
    }catch{
        res.status(404).send("failed to connect to ESP")
        console.log("failed to connect to ESP")
    }
}