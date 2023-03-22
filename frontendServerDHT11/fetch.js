
function fetchDatos(){
    fetch('http://192.168.1.79/full')
  .then(response => response.json())
  .then(data => {
    // update temperatura
    document.getElementById("celcius").innerHTML = "Celcius: " + data["temperaturaC"];

    document.getElementById("fahrenheit").innerHTML = "Farenheit: " + data["temperaturaF"];
    
    // update humedad
    document.getElementById("humedad").innerHTML = "Humedad (%): " + data["humedad"];
    
    // update heat index
    document.getElementById("hicelcius").innerHTML = "HeatIndex Celcius: " + data["heat_indexC"];

    document.getElementById("hifahrenheit").innerHTML = "HeatIndex Farenheit: " + data["heat_indexF"];
  })
  .catch(error => console.error(error));
}

setInterval(fetchDatos, 5000);

window.onload = fetchDatos;