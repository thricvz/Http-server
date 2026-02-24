const button = document.getElementById("button-1")
button.addEventListener("click", send_request)

function send_request() {
    const response = fetch('http://localhost:8080/hello', {
      method : "POST",
      body : "hello to all my niggers" 
    })

    return 0;
}
