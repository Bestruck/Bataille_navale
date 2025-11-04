#!/usr/bin/env python3
"""
Script pour convertir la bannière SVG en PNG pour LinkedIn
"""

try:
    from cairosvg import svg2png
    
    print("🎨 Conversion de la bannière SVG en PNG...")
    
    # Lire le fichier SVG
    with open('linkedin-banner.svg', 'rb') as svg_file:
        svg_data = svg_file.read()
    
    # Convertir en PNG
    svg2png(bytestring=svg_data, 
            write_to='linkedin-banner.png',
            output_width=1584,
            output_height=396)
    
    print("✅ Bannière créée avec succès : linkedin-banner.png")
    print("📏 Dimensions : 1584 x 396 pixels")
    print("📤 Vous pouvez maintenant télécharger ce fichier sur LinkedIn !")
    
except ImportError:
    print("⚠️  Le module cairosvg n'est pas installé.")
    print("📦 Installation en cours...")
    import subprocess
    import sys
    
    try:
        subprocess.check_call([sys.executable, "-m", "pip", "install", "cairosvg"])
        print("✅ cairosvg installé avec succès!")
        print("🔄 Veuillez relancer ce script.")
    except:
        print("❌ Impossible d'installer cairosvg automatiquement.")
        print("\n💡 Solutions alternatives :")
        print("1. Installez manuellement : pip install cairosvg")
        print("2. Ou ouvrez linkedin-banner.svg dans un navigateur")
        print("3. Faites clic droit > Enregistrer l'image sous...")
        print("4. Ou utilisez un convertisseur en ligne : https://cloudconvert.com/svg-to-png")

except Exception as e:
    print(f"❌ Erreur : {e}")
    print("\n💡 Solutions alternatives :")
    print("1. Ouvrez linkedin-banner.svg dans votre navigateur")
    print("2. Faites clic droit > Enregistrer l'image sous...")
    print("3. Ou utilisez un convertisseur en ligne : https://cloudconvert.com/svg-to-png")
