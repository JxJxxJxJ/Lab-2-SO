#!/bin/bash

# Me detengo si cualquier comando retorna error (rc != 0)
set -e

# 1. Verificar que estoy en RAMA_DESEADA (la rama que quiero hacer release)
BRANCH_ACTUAL="$(git rev-parse --abbrev-ref HEAD)"
BRANCH_DESEADA="main" # Por defecto main
if [[ $BRANCH_ACTUAL != $BRANCH_DESEADA ]]; then
  echo "⚠️ Este script debe ejecutarse en la rama $BRANCH_DESEADA. Rama actual: $BRANCH_ACTUAL"
  exit 1
fi

# 2. Veo si hay cambios en el repositorio sin commitear
# -n "String" == True si la cadena es no-vacía
if [[ -n "$(git status --porcelain)" ]]; then
  echo "⚠️ Hay cambios sin confirmar en el repositorio. Haz commit o descarta los cambios antes de continuar."
  exit 1
fi

# # 3. Verificar que los commits cumplen con el formato de Conventional Commits
# echo "✅ Verificando el historial de commits..."
# if cog check --from-latest-tag; then
#   echo "✔️ Historial de commits verificado con éxito."
# else
#   echo "❌ Hay commits no convencionales."
#   exit 1
# fi

# 4. Solicitar al usuario el tipo de incremento de versión
echo "Seleccione el tipo de incremento de versión:"
echo "1) Major"
echo "2) Minor"
echo "3) Patch"
echo "4) Auto"
read -p "Ingrese el número correspondiente (1-4): " opcion

case $opcion in
1)
  BUMP_TYPE="--major"
  ;;
2)
  BUMP_TYPE="--minor"
  ;;
3)
  BUMP_TYPE="--patch"
  ;;
4)
  BUMP_TYPE="--auto"
  ;;
*)
  echo "❌ Opción no válida."
  exit 1
  ;;
esac

# 5. Mostrar el resultado simulado del comando cog bump
echo "🔍 Simulando el incremento de versión con 'cog bump $BUMP_TYPE --dry-run'..."
cog bump $BUMP_TYPE --dry-run

# 6. Confirmar con el usuario antes de proceder
read -p "¿Desea proceder con el incremento de versión? (s/n): " confirmacion
if [[ $confirmacion != "s" ]]; then
  echo "🚫 Operación cancelada por el usuario."
  exit 0
fi

# 7. Realizar el incremento de versión con Cocogitto
echo "🚀 Ejecutando incremento de versión..."
cog bump $BUMP_TYPE

# 8. Obtener la nueva versión generada (ya se crearon el commit y el tag)
VERSION=$(cog -v get-version)
echo "🔖 Version actualizada: $VERSION"

# 9. Subir los cambios y tags a GitHub
echo "📤 Subiendo cambios y tags a GitHub... (O al que sea)"
git push origin main --follow-tags

# 10. Crear o actualizar un release en GitHub utilizando el CHANGELOG.md generado
TAG="v$VERSION"
echo "📦 Procesando release en GitHub para el tag $TAG..."
if gh release view "$TAG" >/dev/null 2>&1; then
  echo "⚠️ El release $TAG ya existe, se actualizará..."
  gh release edit "$TAG" --notes-file CHANGELOG.md --title "$VERSION"
else
  gh release create "$TAG" --title "$VERSION" --notes-file CHANGELOG.md
fi

echo "🎉 ¡Lanzamiento completado con éxito!"
