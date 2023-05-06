
export async function getHomeData(){
    const response = await fetch('http://localhost:3000/api/homeData');
    const data = await response.json();
    return data;
}

export async function getPotValue(){
    const response = await fetch('http://localhost:3000/api/potValue');
    const data = await response.json();
    return data;
}

export async function setLed(request){
    const response = await fetch('http://localhost:3000/api/ledControl', {
        method: 'POST', 
        headers:{
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(request)
    });
    const data = await response.json();
    return data;
}
