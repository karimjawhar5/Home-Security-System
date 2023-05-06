import Image from 'next/image'

import { getHomeData, getPotValue, setLed } from '../api/esp'

import {useState, useEffect} from 'react'

export default function Home() {
  const [potValue, setPotValue] = useState(null)
  const [ledState, setLedState] = useState(null)

  const handleLedSwitch = async ()=>{
    if(ledState == "on"){
      await setLed({state:"off"})
      setLedState("off")
      
    }else{
      await setLed({state:"on"})
      setLedState("on")
    }
  }

  useEffect(()=>{
    async function fetchData(){
      const {led, pot} = await getHomeData()
      if(led == "1"){setLedState("on")}else{setLedState("off")}
      setPotValue(pot)
    }
    fetchData()
  }, [])

  return (
    <div>
      <div className='flex'>
        <p>{ledState}</p>
        <button className='bg-red-500 ml-10 p-1' onClick={handleLedSwitch}>LED SWITCH</button>
      </div>
      <p>{potValue}</p>
    </div>
  )
}

