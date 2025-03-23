interface Button {
  render() : void;
}

interface CheckBox {
  render() : void;
}

class WindowsButton implements Button {
  render(): void {
    console.log("🖱️ Rendering Windows-style button.");
  }
}

class MacButton implements Button {
  render(): void {
    console.log("🍏 Rendering MacOS-style button.");
  }
}

class WindowsCheckbox implements CheckBox {
  render(): void {
    throw new Error("Method not implemented.");
  }
}

class MacCheckbox implements CheckBox {
  render(): void {
    throw new Error("Method not implemented.");
  }
}

interface GUIFactory {
  createButton(): Button;
  createCheckbox(): CheckBox;
}

class WindowsFactory implements GUIFactory {
  createButton(): Button {
    return new WindowsButton();
  }

  createCheckbox(): CheckBox {
    return new WindowsCheckbox();
  }
}

class MacFactory implements GUIFactory {
  createButton(): Button {
    return new MacButton();
  }

  createCheckbox(): CheckBox {
    return new MacCheckbox();
  }
}

function buildUI(factory: GUIFactory) {
  const button = factory.createButton();
  const CheckBox = factory.createCheckbox();

  button.render();
  CheckBox.render();
}

const isWindows = true; 

const factory: GUIFactory = isWindows ? new WindowsFactory() : new MacFactory();
buildUI(factory);