const MAX_SCALE = 50

function initTable () {
  const table = document.createElement('table')
  table.id = 'colors'
  const array = ['SRM', 'sRGB']
  const tr = document.createElement('tr')
  for (let i = 0; i < array.length; ++i) {
    const th = document.createElement('th')
    const text = document.createTextNode(array[i])
    th.appendChild(text)
    tr.appendChild(th)
  }
  table.appendChild(tr)
  for (let i = 1; i <= MAX_SCALE; ++i) {
    const tr = document.createElement('tr')
    tr.style.color = '#ffffff'
    const td1 = document.createElement('td')
    const td2 = document.createElement('td')
    const text1 = document.createTextNode(i.toString())
    const text2 = document.createTextNode('')
    td1.appendChild(text1)
    td2.appendChild(text2)
    tr.appendChild(td1)
    tr.appendChild(td2)
    table.appendChild(tr)
  }
  document.body.appendChild(table)
}

function getPath () {
  return document.getElementById('pathselect').value
}

function updatePathDisplay () {
  document.getElementById('path').value = getPath()
}

function updateTable () {
  const path = getPath()
  const table = document.getElementById('colors')
  for (let i = 1; i <= MAX_SCALE; ++i) {
    const color = rgbToHex(srmToSRGB(i, path))
    const row = table.rows[i]
    row.style.backgroundColor = color
    row.cells[1].innerHTML = color
  }
}

window.onload = (event) => {
  document.getElementById('pathselect').value = DEFAULT_PATH
  updatePathDisplay()
  initTable()
  updateTable()

  document.getElementById('pathselect').oninput = () => {
    updatePathDisplay()
    updateTable()
  }
}
