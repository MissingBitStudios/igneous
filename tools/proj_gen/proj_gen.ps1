param (
	[string]$name = "proj_name"
)
if(-not($name)) { Throw “You must supply a value for -name” }
$proj_dir = ([IO.Path]::Combine($(Get-Location), $name))
New-Item -ItemType Directory -Force -Path $proj_dir | Out-Null
(Get-Content ([IO.Path]::Combine($PSScriptRoot, 'template', 'CMakeLists.txt'))) -replace '{% name %}', $name | Set-Content ([IO.Path]::Combine($proj_dir, 'CMakeLists.txt'))